.. index::
   single: Forms; Fields; money

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/11

money フィールドタイプ
======================

テキストフィールドを表示し、送信された通貨情報を取り扱うことに特化しています。

このフィールドタイプはテキストフィールドの横に表示される通貨記号を指定することができます。データの入出力の処理方法をカスタマイズするための他のいくつかのオプションもあります。

+-------------+---------------------------------------------------------------------+
| 対応するタグ| ``input`` ``text`` フィールド                                       |
+-------------+---------------------------------------------------------------------+
| オプション  | - `currency`_                                                       |
|             | - `divisor`_                                                        |
|             | - `precision`_                                                      |
|             | - `grouping`_                                                       |
+-------------+---------------------------------------------------------------------+
| 継承された  | - `required`_                                                       |
| オプション  | - `label`_                                                          |
|             | - `read_only`_                                                      |
|             | - `disabled`_                                                       |
|             | - `error_bubbling`_                                                 |
|             | - `error_mapping`_                                                  |
|             | - `invalid_message`_                                                |
|             | - `invalid_message_parameters`_                                     |
|             | - `mapped`_                                                         |
+-------------+---------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                           |
+-------------+---------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\MoneyType` |
+-------------+---------------------------------------------------------------------+

フィールドオプション
--------------------

currency
~~~~~~~~

**データ型**: ``string`` **デフォルト**: ``EUR``

お金が特定される通貨を指定します。これはテキストボックスによって表示されるべき通貨記号を決定します。通貨によって通貨記号はテキストフィールドの前か後に表示されます。
    
これは `3 letter ISO 4217 code`_ の中の任意のものです。 ``false`` にして通貨記号を隠すことも出来ます。

divisor
~~~~~~~

**データ型**: ``integer`` **デフォルト**: ``1``

何らかの理由で、表示前に開始値を除算する必要がある場合は、 ``divisor`` オプションを使用することができます。
例::


    $builder->add('price', 'money', array(
        'divisor' => 100,
    ));

上記の場合で ``price`` フィールドに ``9900`` がセットされたら、実際には ``99`` が表示されます。ユーザーが値として ``99`` を送信したとき、 ``100`` が掛けられ、結果として ``9900`` がオブジェクトに戻されます。

precision
~~~~~~~~~

**データ型**: ``integer`` **デフォルト**: ``2``

何らかの理由で、小数点第2位以外が必要な場合、この値を変更することが出来ます。たとえば（精度を0にして）最も近いドルに切り上げしたいという以外、必要ないでしょう。

.. include:: /reference/forms/types/options/grouping.rst.inc

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. _`3 letter ISO 4217 code`: http://ja.wikipedia.org/wiki/ISO_4217

.. 2013/12/11 yositani2002 4848f40b7de1463e40911bc2871d8990757d0097
