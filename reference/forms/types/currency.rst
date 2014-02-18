.. index::
    single: Forms; Fields; currency

.. note::

   * 対象バージョン：2.3
   * 翻訳更新日：2014/02/16


currency フィールドタイプ
=========================

``currency`` タイプは :doc:`choice タイプ </reference/forms/types/choice>` のサブセットで、
ユーザーは `3-letter ISO 4217`_ の大量の通貨リストから選択することができます。

``choice`` タイプとは異なり、\ ``choices`` または ``choice_list`` オプションを特定する必要はなく、
フィールドタイプとして自動的に大量の通貨リストを使います。それらのオプションのどちらかを手動で設定する
ことは *出来ます* が、その場合は ``choice`` タイプを直接使うべきです。

+-------------+------------------------------------------------------------------------+
| 対応するタグ| 様々なタグとして利用可能 (see :ref:`forms-reference-choice-tags`)      |
+-------------+------------------------------------------------------------------------+
| 上書きされた| - `choices`_                                                           |
| オプション  |                                                                        |
+-------------+------------------------------------------------------------------------+
| 継承された  | - `multiple`_                                                          |
| オプション  | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `error_bubbling`_                                                    |
|             | - `empty_data`_                                                        |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `label_attr`_                                                        |
|             | - `data`_                                                              |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| 親タイプ    | :doc:`choice </reference/forms/types/choice>`                          |
+-------------+------------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CurrencyType` |
+-------------+------------------------------------------------------------------------+

上書きされたオプション
----------------------

choices
~~~~~~~

**デフォルト**: ``Symfony\Component\Intl\Intl::getCurrencyBundle()->getCurrencyNames()``

choiscesオプションはデフォルトですべての通貨になります。

継承されたオプション
--------------------

以下のオプションは :doc:`choice</reference/forms/types/choice>` タイプを継承しています:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

以下のオプションは :doc:`form</reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. _`3-letter ISO 4217`: http://ja.wikipedia.org/wiki/ISO_4217

.. 2013/11/24 yositani2002 3842f599eb277e387c820816c91a099d6be50df2
.. 2014/02/16 yositani2002 d9c6a853a84754e8fc8379bf61b70c02493e305f
