.. index::
   single: Forms; Fields; choice

.. note::

   * 対象バージョン：2.4 (2.0以降)
   * 翻訳更新日：2013/12/30

entity フィールドタイプ
=======================

``Doctrine`` の ``entity`` から選択肢を読み込む様に設計された特別な ``choice`` フィールドです。例えば、 ``Category`` エンティティを持つ場合、 ``select`` フィールドにデータベースからの ``Category`` オブジェクト全て、または一部を表示するのにこのフィールドを使います。

+-------------+------------------------------------------------------------------+
| 対応するタグ| さまざまなタグに対応 (参照 :ref:`forms-reference-choice-tags`)   |
+-------------+------------------------------------------------------------------+
| オプション  | - `class`_                                                       |
|             | - `property`_                                                    |
|             | - `group_by`_                                                    |
|             | - `query_builder`_                                               |
|             | - `em`_                                                          |
+-------------+------------------------------------------------------------------+
| 上書きされた| - `choices`_                                                     |
| オプション  | - `choice_list`_                                                 |
+-------------+------------------------------------------------------------------+
| 継承された  | - `required`_                                                    |
| オプション  | - `label`_                                                       |
|             | - `multiple`_                                                    |
|             | - `expanded`_                                                    |
|             | - `preferred_choices`_                                           |
|             | - `empty_value`_                                                 |
|             | - `read_only`_                                                   |
|             | - `disabled`_                                                    |
|             | - `error_bubbling`_                                              |
|             | - `error_mapping`_                                               |
|             | - `mapped`_                                                      |
+-------------+------------------------------------------------------------------+
| 親タイプ    | :doc:`choice </reference/forms/types/choice>`                    |
+-------------+------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Bridge\\Doctrine\\Form\\Type\\EntityType`       |
+-------------+------------------------------------------------------------------+

基本的な使い方
--------------

``entity`` タイプは一つだけ必須のオプションを持ちます。選択フィールドの内側に一覧にされるべきエンティティです。::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'property' => 'username',
    ));

この場合、すべての ``User`` オブジェクトはデータベースから読み取られ、セレクトタグ、ラジオボタン、または、一連のチェックボックスとして表示されます（これは ``multiple`` と ``expanded`` の値によります）。
もし、エンティティオブジェクトが ``__toString()`` メソッドを持っていない場合、 ``property`` オプションを設定する必要があります。

エンティティにカスタムクエリを利用する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

エンティティを取り出す際に、特別なカスタムクエリーを定義する必要がある場合（一部のエンティティのみ欲しい場合、または、並べ順を指定する必要がある場合など）、 ``query_builder`` オプションを利用します。以下のように使うのが最も簡便です。::

    use Doctrine\ORM\EntityRepository;
    // ...

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'query_builder' => function(EntityRepository $er) {
            return $er->createQueryBuilder('u')
                ->orderBy('u.username', 'ASC');
        },
    ));

.. _reference-forms-entity-choices:

Choices の利用
~~~~~~~~~~~~~~

すでに選択肢に含めるエンティティの正確な集合を持っている場合、簡潔に ``choices`` キーに渡すことが出来ます。
例えば、（フォームオプションとしてフォームにわたされた） ``$group`` 変数を持ち、 ``getUsers`` は ``User`` エンティティのコレクションをかえす場合、直接 ``choices`` オプションに提供できます。::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'choices' => $group->getUsers(),
    ));

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

フィールドオプション
--------------------

class
~~~~~

**データ型**: ``string`` **必須**

エンティティのクラスです(例 ``AcmeStoreBundle:Category`` )。これは完全修飾クラス名(例 ``Acme\StoreBundle\Entity\Category`` )か、（上述のような）省略名です。

property
~~~~~~~~

**データ型**: ``string``

これはエンティティが ``HTML`` 要素にテキストとして表示されるために使用されるプロパティです。未設定のままの場合、エンティティオブジェクトは文字列変換されるので、 ``__toString()`` メソッドを持つ必要があります。

group_by
~~~~~~~~

**データ型**: ``string``

グループで選択できるよう整理する、プロパティパス（例 ``author.name`` ）です。セレクトタグとして表示される場合のみ動作し、選択肢を囲むように ``optgroup`` タグを追加する動作をします。このプロパティパスの値を返さない選択肢は、直接セレクトタグの下に表示され、 ``optgroup`` で囲まれません。

query_builder
~~~~~~~~~~~~~

**データ型**: ``Doctrine\ORM\QueryBuilder`` または 単一の無名関数

指定した場合、フィールドに使用される選択肢の絞込み（とその順序）のクエリーに使用されます。このオプションの値は ``QueryBuilder`` オブジェクトか無名関数です。無名関数を使用する場合は、引数としてエンティティの ``EntityRepository`` を一つとります。

em
~~

**データ型**: ``string`` **デフォルト**: デフォルトエンティティマネージャー

指定された場合、デフォルトのエンティティマネージャの代わりに指定されたエンティティマネージャが選択肢の読み込みに使われます。

上書きされたオプション
----------------------

choices
~~~~~~~

**データ型**:  array || ``\Traversable`` **デフォルト**: ``null``

`class`_ と `query_builder`_ オプションでエンティティを読み込む代わりに、直接 ``choices`` オプションを渡すことが出来ます。
参照 :ref:`reference-forms-entity-choices` 。

choice_list
~~~~~~~~~~~

**デフォルト**: :class:`Symfony\\Bridge\\Doctrine\\Form\\ChoiceList\\EntityChoiceList`

``entity`` タイプの目的は、上述のすべてのオプションを利用して ``EntityChoiceList`` を作成し設定することです。このオプションを上書きする必要がある場合は、  :doc:`/reference/forms/types/choice` を直接使用することも検討できるでしょう。

継承されたオプション
--------------------

以下のオプションは :doc:`choice </reference/forms/types/choice>` タイプを継承しています:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. note::
    
    ``Doctrine`` のエンティティのコレクションを使用している場合、 :doc:`/reference/forms/types/collection` のドキュメントを読むとより参考になります。加えて、クックブックの記事に完全な例があります。
    :doc:`/cookbook/form/form_collections`
    
.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/12/30 yositani2002 e2307a93adb0f21593c054d34381d81b11e68da3
