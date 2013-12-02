.. index::
   single: Forms; Fields; collection

.. note::

    * 対象バージョン : 2.3
    * 翻訳更新日 : 2013/12/01

collection フィールドタイプ
===========================

このフィールドタイプは複数のフィールドやフォームの"コレクション"の表示に使われます。 
最も簡単なところでは、複数のメールアドレス用フィールドを表示するために、テキストフィールドの配列をマップできます。
より複雑な例では、フォーム全体をコレクションの要素として扱うこともできます。
たとえば、製品情報と製品に関連する多くの写真を、1つの製品情報フォームにまとめて管理したい場合に便利です。

+-------------+-----------------------------------------------------------------------------+
| 対応するタグ| `type`_ オプションに依存                                                    |
+-------------+-----------------------------------------------------------------------------+
| オプション  | - `type`_                                                                   |
|             | - `options`_                                                                |
|             | - `allow_add`_                                                              |
|             | - `allow_delete`_                                                           |
|             | - `prototype`_                                                              |
|             | - `prototype_name`_                                                         |
+-------------+-----------------------------------------------------------------------------+
| 継承された  | - `label`_                                                                  |
| オプション  | - `error_bubbling`_                                                         |
|             | - `error_mapping`_                                                          |
|             | - `by_reference`_                                                           |
|             | - `empty_data`_                                                             |
|             | - `mapped`_                                                                 |
+-------------+-----------------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                                   |
+-------------+-----------------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CollectionType`    |
+-------------+-----------------------------------------------------------------------------+

.. note::

    もしDoctrineのエンティティとあわせて使っている場合、 `allow_add`_ 、 `allow_delete`_ 、 `by_reference`_ オプションには特に注意を払ってください。
    クックブックの記事「:doc:`/cookbook/form/form_collections`」に完全な例があります。

基本的な使い方
--------------

このタイプは、ひとつのフォーム中にある類似アイテムのコレクションを管理したいときに使われます。
たとえば、メールアドレス配列に対応する ``emails`` フィールドがあるとします。フォーム中では1つのメールアドレスにつきテキストボックスを1つずつ出力したいです::

    $builder->add('emails', 'collection', array(
        //配列の中のどのアイテムも "email" フィールドとされます
        'type'   => 'email',
        // これらのオプションはすべて "email" タイプに渡されます
        'options'  => array(
            'required'  => false,
            'attr'      => array('class' => 'email-box')
        ),
    ));

一番簡単に表示する方法はすべてを一度で行うことです:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.emails) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']) ?>

より柔軟な方法は以下のようになります:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.emails) }}
        {{ form_errors(form.emails) }}

        <ul>
        {% for emailField in form.emails %}
            <li>
                {{ form_errors(emailField) }}
                {{ form_widget(emailField) }}
            </li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <?php echo $view['form']->label($form['emails']) ?>
        <?php echo $view['form']->errors($form['emails']) ?>

        <ul>
        <?php foreach ($form['emails'] as $emailField): ?>
            <li>
                <?php echo $view['form']->errors($emailField) ?>
                <?php echo $view['form']->widget($emailField) ?>
            </li>
        <?php endforeach; ?>
        </ul>

どちらの場合も ``emails`` データ配列が空の場合は入力フィールドは表示されません。

この簡単な例では、まだ新しいアドレスを追加することや既存のアドレスを削除することはできません。
新しくアドレスを追加するには `allow_add`_ オプション（および `prototype`_ オプション）を有効にしてください。
(続く例を見てください)。メールアドレスを ``emails`` 配列から削除するには `allow_delete`_ オプションを有効にしてください。

アイテムの追加と削除
~~~~~~~~~~~~~~~~~~~~

もし `allow_add` が ``true`` にセットされていれば、認識されていないアイテムが送信されたとき、
それらはシームレスにアイテム配列に追加されるでしょう。これは理論上はすばらしいですが、
実際上、クライアントサイドJavaScriptを正しくするにはもう少し手間がかかります。

上述の例に沿って続く例では、 ``emails`` データ配列に二つのメールアドレスがある状態で始めるとします。
その場合には、2つの入力のフィールドは（フォームの名前に応じて）次のようにレンダリングされます:

.. code-block:: html

    <input type="email" id="form_emails_0" name="form[emails][0]" value="foo@foo.com" />
    <input type="email" id="form_emails_1" name="form[emails][1]" value="bar@bar.com" />

ユーザーが別のメールを追加できるようにするために、まず `allow_add`_ を ``true`` に設定します。そして、
- JavaScript で - ``form[emails][2]`` という名前で追加のフィールド（必要であればさらに追加のフィールド）を動的にレンダリングします。

これを容易にするため、 `prototype`_ オプションを ``true`` にすることで"テンプレート" フィールドの表示を許可します。
それは JavaScript を利用して動的に新しいフィールドを作成するために利用できます。
表示されたプロトタイプフィールドは次のようになります::

.. code-block:: html

    <input type="email" id="form_emails___name__" name="form[emails][__name__]" value="" />

``__name__`` をいくつかの固有の値で置き換えることで （例えば ``2`` ）、
フォームに新しいHTMLフィールドを構築、挿入することができます。

jQueryを使用した簡単な例は次のようになります。フィールドのコレクションを一度にすべて表示するなら（例えば ``form_row(form.emails)`` ）、
 ``data-prototype`` 属性は自動的に表示されるためより簡単です（多少の違いについては続きのnoteを参照のこと）。
これまで説明したことをすべて記述した JavaScript コードは、次のようになります:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_start(form) }}
            {# ... #}

            {# data-prototype 属性にプロトタイプを保存してください #}
            <ul id="email-fields-list" data-prototype="{{ form_widget(form.emails.vars.prototype) | e }}">
            {% for emailField in form.emails %}
                <li>
                    {{ form_errors(emailField) }}
                    {{ form_widget(emailField) }}
                </li>
            {% endfor %}
            </ul>

            <a href="#" id="add-another-email">Add another email</a>

            {# ... #}
        {{ form_end(form) }}

        <script type="text/javascript">
            // どれだけのメールフィールドが表示したかを追跡しつづけてください。
            var emailCount = '{{ form.emails | length }}';

            jQuery(document).ready(function() {
                jQuery('#add-another-email').click(function() {
                    var emailList = jQuery('#email-fields-list');

                    // prototype template を取得してください
                    var newWidget = emailList.attr('data-prototype');
                    // id と プロトタイプの名前で使われる "__name__" をメールアドレスに対して固有の数字で置き換えてください。
                    // 最後の name 属性は name="contact[emails][2]" のようになります
                    newWidget = newWidget.replace(/__name__/g, emailCount);
                    emailCount++;

                    // 新規リスト要素を作成し、リストに追加してください。
                    var newLi = jQuery('<li></li>').html(newWidget);
                    newLi.appendTo(jQuery('#email-fields-list'));

                    return false;
                });
            })
        </script>

.. tip::

    一度にコレクション全体を表示している場合、コレクションを囲んでいる要素（例えば ``div`` または ``table``）の ``data-prototype`` 属性にあるプロトタイプは自動的に利用可能です。
    唯一の違いは、 "form row" 全体はあなたのために表示します。それは上記でなされたように任意のコンテナ要素でラップする必要が無いことを意味します。

フィールドオプション
--------------------

type
~~~~

**データ型**: ``string`` または :class:`Symfony\\Component\\Form\\FormTypeInterface` **必須**

これはこのコレクションの中のそれぞれのアイテムのフィールドタイプです（たとえば ``text``, ``choice`` など）。
たとえば、メールアドレスの配列を持っていた場合、あなたは :doc:`email </reference/forms/types/email>`  タイプを使うでしょう。
もし、その他のフォームのコレクションを組み込みたいとき、新しいフォームタイプのインスタンスを生成し、このオプションに渡してください。

options
~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

これは、 `type`_ オプションで特定されたフォームタイプに渡された配列です。
たとえば、 :doc:`choice </reference/forms/types/choice>` オプションを `type`_ オプションとして
使用した場合（たとえば、ドロップダウンメニューなど）、少なくても選択肢の基になる ``choices`` オプションを
指定する必要があります::

    $builder->add('favorite_cities', 'collection', array(
        'type'   => 'choice',
        'options'  => array(
            'choices'  => array(
                'nashville' => 'Nashville',
                'paris'     => 'Paris',
                'berlin'    => 'Berlin',
                'london'    => 'London',
            ),
        ),
    ));

allow_add
~~~~~~~~~

**データ型**: ``Boolean`` **デフォルト**: ``false``

もし ``true`` にセットした場合、認識していないアイテムがコレクションに送信された場合、新しいアイテムとして追加されます。
送信されたデータの中で、配列の末尾には新しく生成されたアイテムと同じように既存のアイテムを含みます。
より詳細については上述の例を参照してください。

`prototype`_ オプションはプロトタイプアイテムを表示するのに使うことができます。
それらは - JavaScriptと一緒に - 新しいフォームアイテムをクライアントサイドで動的に生成するのに使うことができます。
詳細は上述の例とあわせて :ref:`cookbook-form-collections-new-prototype` を参照してください。

.. caution::

    もし他のフォーム全体をデータベースの1対多の関連を反映して埋め込もうとする場合、
    新しいオブジェクトの外部キーが正しくセットされたか手動で確認する必要があるでしょう。
    Doctrineを使っている場合、これは自動的にはされません。詳細は上述のリンクを参照してください。

allow_delete
~~~~~~~~~~~~

**データ型**: ``Boolean`` **デフォルト**: ``false``

もし ``true`` にセットした場合で既存のアイテムが送信データに含まれていない場合、それはアイテム配列の最後から正しく欠落します。
これは、JavaScript 経由でフォーム要素を DOM から単純に取り除く "delete" ボタンを継承することができることを意味します。
ユーザーがフォームを送信したとき、その送信されたデータからの欠落は、それが最後の配列から削除されたことを意味します。

詳細は :ref:`cookbook-form-collections-remove` を参照してください。

.. caution::

    オブジェクトのコレクションを埋め込んでいるとき、このオプションを使うときは気をつけてください。
    この場合、いくつかの組み込まれたフォームが削除されれば、それらはオブジェクト配列の末尾から正しく消える *でしょう* 。
    しかし、あなたのアプリケーションのロジックしだいでは、オブジェクトが削除されたとき、そのオブジェクトまたは外部キー参照先のメインオブジェクトを削除したいと思うでしょう。。
    これは自動的には取り扱われません。詳細は「 :ref:`cookbook-form-collections-remove` 」を参照してください。

prototype
~~~~~~~~~

**データ型**: ``Boolean`` **デフォルト**: ``true``

このオプションは `allow_add`_ オプションを使うときに便利です。もし ``true`` (且つ ``allow_add`` も ``true`` )の場合、
特別な "prototype" 属性が利用可能になります。それは、新しい要素としてページに見えるべき"テンプレート"の例を表示できます。
この要素に与えられた ``name`` 属性は ``__name__`` です。これはJavaScript 経由でプロトタイプを読み込み、 ``__name__`` を固有の名前又は数字に置き換え、フォーム中に表示する "add another" ボタンを追加することを許可します。
送信されたときに `allow_add`_ オプションの基となる配列にそれは追加されます。

プロトタイプフィールドはコレクションフィールドの中の ``prototype`` 変数を経て表示されることができます:

.. configuration-block::

    .. code-block:: jinja

        {{ form_row(form.emails.vars.prototype) }}

    .. code-block:: php

        <?php echo $view['form']->row($form['emails']->vars['prototype']) ?>

ここで留意すべきは、あなたが本当に必要なのは "widget" ですが、どのようにフォームを表示するか次第では、全体を持っている "form row" の方がより簡単かもしれないことです。

.. tip::

    コレクションフィールド全体を一度で表示する場合、プロトタイプの form row はコレクションを囲む（ ``div`` または ``table`` ）要素の中の
     ``data-prototype`` 属性で自動的に利用可能です。 

このオプションを実際にどのように利用するかより詳しくは上述の例だけではなく「 :ref:`cookbook-form-collections-new-prototype` 」をご覧ください。

prototype_name
~~~~~~~~~~~~~~

**データ型**: ``String`` **デフォルト**: ``__name__``

いくつかのコレクションをフォームの中で持つ場合、またはひどくネストしたコレクションを持つ場合、
関係のないプレースホルダは同じ値に置き換えられないためプレースホルダを変えたいと思うかもしれません。


継承したオプション
------------------


以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています。
すべてのオプションはここにリストしてはおらず、このタイプにもっとも当てはまるもののみです:

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

error_bubbling
~~~~~~~~~~~~~~

**データ型**: ``Boolean`` **デフォルト**: ``true``

.. include:: /reference/forms/types/options/_error_bubbling_body.rst.inc

.. _reference-form-types-by-reference:

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. 2013/12/01 yositani2002 d11327b2c28ebb71b9cdc1b5cf5879183905b3ad
